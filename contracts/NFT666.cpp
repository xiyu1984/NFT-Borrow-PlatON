#include "NFT666.hpp"
#include <platon/panic.hpp>

void contract_throw(const std::string& msg)
{
    ::platon_debug((const unsigned char*)msg.c_str(), msg.size());
    ::platon_revert();
}

void NFT666::init()
{

}

void NFT666::lendUsageTo(const std::string& to, const std::string& token_id, uint64_t period)
{
    auto toAddress = platon::make_address(to);
    if (!toAddress.second)
    {
        contract_throw("Invalid `to` account!");
    }
    
    platon::Address pre_account = platon::platon_caller();

    if (!owner_ship.contains(token_id))
    {
        contract_throw("token does not exist!");
    }
    auto art = &owner_ship[token_id];
    if (!((art->ownership == pre_account) && (art->usage_rights == pre_account)))
    {
        contract_throw("Lend Unauthorized!");
    }
    
    if (!assets_usage_info.contains(art->usage_rights))
    {
         contract_throw("There's a bug, because someone has the usage_right, but the asset does not existed in the asset_usage_info table!");
    }
    auto cur_usage_tokens = &assets_usage_info[art->usage_rights];
    // delete from current usage
    cur_usage_tokens->erase(token_id);

    // add into new usage
    assets_usage_info[toAddress.first].insert(token_id);
    
    // change usage_rights
    art->usage_rights = toAddress.first;

    // delete from approved
    usage_approvals.erase(token_id);

    // add leasing record
    leasing_period[token_id] = ::platon_block_number() + period;
}

void NFT666::usageReturn(const std::string& token_id)
{
    auto pre_account = platon::platon_caller();

    if (!owner_ship.contains(token_id))
    {
        contract_throw("token does not exist!");
    }
    auto art = &owner_ship[token_id];
    
    if (pre_account != art->usage_rights)
    {
        if (pre_account != art->ownership)
        {
            contract_throw("Return Unauthorized!");
        }
        else{
            if (!leasing_period.contains(token_id))
            {
                contract_throw("No leasing record!");
            }
            auto token_lease_period = leasing_period[token_id];
            if (::platon_block_number() <= token_lease_period)
            {
                contract_throw("Return Unauthorized. Not time up!");
            }
        }
    }
    
    // let mut cur_usage_tokens = self.assets_usage_info.get(&art.usage_rights).expect("There's a bug, because someone has the usage_right, but the asset does not existed in the asset_usage_info table!");
    if (!assets_usage_info.contains(art->usage_rights))
    {
        contract_throw("There's a bug, because someone has the usage_right, but the asset does not existed in the asset_usage_info table!");
    }
    auto cur_usage_tokens = &assets_usage_info[art->usage_rights];

    // delete from current usage
    cur_usage_tokens->erase(token_id);

    // add into new usage
    assets_usage_info[art->ownership].insert(token_id);

    // change usage_rights
    art->usage_rights = art->ownership;

    // delete from approved
    usage_approvals.erase(token_id);

    // add leasing record
    leasing_period.erase(token_id);
}

uint64_t NFT666::getLeasingPeriod(const std::string& token_id)
{
    if (leasing_period.contains(token_id))
    {
        return leasing_period[token_id];
    }
    
    return uint64_t(0);
}

platon::u128 NFT666::balanceOf(const std::string& _owner)
{
    auto account = platon::make_address(_owner);
    if (!account.second)
    {
        contract_throw("None of the account!");
    }
    
    return assets_own_info[account.first].size();
}

std::string NFT666::ownerOf(const std::string& _tokenId)
{
    if (!owner_ship.contains(_tokenId))
    {
        contract_throw("The _tokenId dose not exist!");
    }
    
    return owner_ship[_tokenId].ownership.toString();
}

void NFT666::safeTransferFromWithData(const std::string& _from, const std::string& _to, const std::string& _tokenId, platon::bytes data)
{
    // self.transferFrom(from.clone(), to.clone(), token_id.clone());
    transferFrom(_from, _to, _tokenId);

    // cross contract call
    auto toAddress = platon::make_address(_to);
    if (!toAddress.second)
    {
        contract_throw("Invalid `_to` address!");
    }
    
    auto result = platon::platon_call(toAddress.first, (unsigned int)(0), (unsigned int)(0), "onERC721Received", 
                                                                                        platon::platon_caller().toString(),
                                                                                        _from,
                                                                                        _tokenId, 
                                                                                        data);
}

void NFT666::safeTransferFrom(const std::string& _from, const std::string& _to, const std::string& _tokenId)
{
    safeTransferFromWithData(_from, _to, _tokenId, platon::asBytes(""));
}

void NFT666::transferFrom(const std::string& _from, const std::string& _to, const std::string& _tokenId)
{
    auto toAddress = platon::make_address(_to);
    if (!toAddress.second)
    {
        contract_throw("Invalid `to` address!");
    }
    

    if (!owner_ship.contains(_to))
    {
        contract_throw("`token_id` not exist!");
    }
    auto art = &owner_ship[_to];
    
    // token without approves

    // the owner of the token must be the same as `from`
    if (_from != art->ownership.toString())
    {
        contract_throw("Ownership Unauthorized!");
    }

    // if in ownership, the `from` must be the same as the `predecessor_account_id`
    // if not, the `predecessor_account_id` must be in the `self.approvals` of the token
    auto pre_account = platon::platon_caller();

    if (pre_account != art->ownership)
    {
        if (!approvals.contains(_tokenId))
        {
            contract_throw("Caller Ownership Unauthorized: 1");
        }

        if (approvals[_tokenId] != pre_account)
        {
            contract_throw("Caller Ownership Unauthorized: 2");
        }
    }
    
    // process leasing first
    if (!leasing_period.contains(_tokenId))
    {
        if (art->ownership != art->usage_rights)
        {
            contract_throw("Usage Unauthorized: if there's no leasing, the `ownership` must be the same as `usage_rights`");
        }
        
        transfer_usage_without_check(_from, _to, _tokenId);
    }
    
    transfer_ownership_without_check(_from, _to, _tokenId);
}

void NFT666::approve(const std::string& _approved, const std::string& _tokenId)
{
    auto apvAddress = platon::make_address(_approved);
    if (!apvAddress.second)
    {
        contract_throw("Invalide `_approved` address!");
    }
    
    if (!owner_ship.contains(_tokenId))
    {
        contract_throw("Token does not exist!");
    }
    auto owner = owner_ship[_tokenId];

    if (platon::platon_caller() != owner.ownership)
    {
        contract_throw("Ownership Unauthorized");
    }
    
    approvals[_tokenId] = apvAddress.first;
}

void NFT666::setApprovalForAll(const std::string& _operator, bool _approved)
{
    auto pre_account = platon::platon_caller();
    auto oper = platon::make_address(_operator);
    if (!oper.second)
    {
        contract_throw("invalid `_operator`");
    }
    
    if (pre_account == oper.first)
    {
        return;
    }
    
    if (!assets_own_info.contains(pre_account))
    {
        contract_throw("None of assets!");
    }
    auto assets_owned = &assets_own_info[pre_account];
    
    if (_approved)
    {
        for (auto itr = assets_owned->begin(); itr != assets_owned->end(); ++itr)
        {
            approvals[*itr] = oper.first;
        }
    }
    else{
        for (auto itr = assets_owned->begin(); itr != assets_owned->end(); ++itr)
        {
            approvals.erase(*itr);
        }
    }
}

std::string NFT666::getApproved(const std::string& _tokenId)
{
    if (!approvals.contains(_tokenId))
    {
        contract_throw("`_tokenId` does not exist!");
    }
    
    return approvals[_tokenId].toString();
}

bool NFT666::isApprovedForAll(const std::string& _owner, const std::string& _operator)
{
    // let assets_owned = self.assets_own_info.get(&owner).expect("None of assets!");
    auto ownerAddress = platon::make_address(_owner);
    if (!ownerAddress.second)
    {
        return false;
    }
    
    if (!assets_own_info.contains(ownerAddress.first))
    {
        contract_throw("None of assets!");
    }
    auto assets_owned = assets_own_info[ownerAddress.first];
    

    // for token in assets_owned.iter(){
    //     let approved_opt = self.approvals.get(&token);
    //     if let Some(approved) = approved_opt {
    //         if approved != operator{
    //             return false;
    //         }
    //     }else{
    //         return false;
    //     }
    // }

    for (auto itr = assets_owned.begin(); itr != assets_owned.end(); ++itr)
    {
        if (!approvals.contains(*itr))
        {
            return false;
        }
        
        if (approvals[*itr].toString() != _operator)
        {
            return false;
        }
    }
    
    return true;
}

uint64_t NFT666::totalSupply()
{
    return total_supply.self();
}

std::string NFT666::name()
{
    return contract_meta.self().name;
}

std::string NFT666::symbol()
{
    return contract_meta.self().symbol;
}

std::string NFT666::tokenURI(const std::string& _tokenId)
{
    if (!tokens.contains(_tokenId))
    {
        contract_throw("None of the token_id");
    }
    auto v = tokens[_tokenId];
    if (v.media.empty())
    {
        return "";
    }
    else{
        return v.media[0];
    }
}

void NFT666::transfer_usage_without_check(const std::string& from, const std::string& to, const std::string& token_id)
{
    auto fromAddress = platon::make_address(from);
    auto toAddress = platon::make_address(to);
    if ((!fromAddress.second) || (!toAddress.second))
    {
        contract_throw("Invalid from or to Address");
    }

    if (!owner_ship.contains(token_id))
    {
        contract_throw("token dose not exist!");
    }
    auto asset_right = &owner_ship[token_id];

    if (!assets_usage_info.contains(fromAddress.first))
    {
        contract_throw("There's a bug, because someone has the usage_right, but the asset does not existed in the asset_usage_info table!");
    }
    auto cur_usage_tokens = &assets_usage_info[fromAddress.first];

    // delete from current usage
    cur_usage_tokens->erase(token_id);

    // add into new usage
    assets_usage_info[toAddress.first].insert(token_id);

    // change usage_rights
    asset_right->usage_rights = toAddress.first;

    // delete from approved
    usage_approvals.erase(token_id);
}

void NFT666::transfer_ownership_without_check(const std::string& from, const std::string& to, const std::string& token_id)
{
    auto fromAddress = platon::make_address(from);
    auto toAddress = platon::make_address(to);
    if ((!fromAddress.second) || (!toAddress.second))
    {
        contract_throw("Invalid from or to Address");
    }

    if (!owner_ship.contains(token_id))
    {
        contract_throw("token dose not exist!");
    }
    auto art = &owner_ship[token_id];
    
    // update `self.assets_own_info`
    if (!assets_own_info.contains(fromAddress.first))
    {
        contract_throw("There's a bug, because someone has the ownership, but the asset dose not existed in the asset_own_info table!");
    }
    auto cur_owned_tokens = &assets_own_info[fromAddress.first];
    
    // delete from current owner
    cur_owned_tokens->erase(token_id);

    // add into new owner
    assets_own_info[toAddress.first].insert(token_id);

    // update `self.owner_ship`
    art->ownership = toAddress.first;

    // delete from approved
    approvals.erase(token_id);
}

NFT666Token NFT666::mint(AssetRights asset_rights, TokenMetaData token_metadata)
{
    assert_eq!(env::predecessor_account_id(), self.owner_id, "Unauthorized");

    if !env::is_valid_account_id(asset_rights.ownership.as_bytes()){
        env::panic_str("invalid owner account!");
    }

    if !env::is_valid_account_id(asset_rights.usage_rights.as_bytes()){
        env::panic_str("invalid usage account!");
    }

    assert_eq!(asset_rights.ownership , asset_rights.usage_rights, "the `ownership` must be the same as `usage_rights` in `mint`");

    self.total_supply += 1;

    // create the unique token_id
    let token_id: String = token_metadata.to_hex_string();
    let token_id = token_id + env::current_account_id().as_str();
    let token_id = format!("{}+{}", token_id, self.total_supply);
    let token_id = hex::encode(env::sha256(token_id.as_bytes()));

    // the token_id must be unique!
    if self.tokens.contains_key(&token_id){
        env::panic_str("cannot create, check the metadata of the token!");
    }

    // insert tokens
    self.tokens.insert(&token_id, &token_metadata);

    // insert ownership
    self.owner_ship.insert(&token_id, &asset_rights);

    // insert assets_own_info
    let mut owned_tokens = self.assets_own_info.get(&asset_rights.ownership).unwrap_or_else(||{
        UnorderedSet::new(StorageRecord::AssetsOwnTable {
            account_hash: env::sha256(asset_rights.ownership.as_bytes()),
        })
    });
    owned_tokens.insert(&token_id);
    self.assets_own_info.insert(&asset_rights.ownership, &owned_tokens);

    // insert assets_usage_info
    let mut usage_tokens = self.assets_usage_info.get(&asset_rights.usage_rights).unwrap_or_else(||{
        UnorderedSet::new(StorageRecord::AssetsUsageTable{
            account_hash: env::sha256(asset_rights.usage_rights.as_bytes()),
        })
    });
    usage_tokens.insert(&token_id);
    self.assets_usage_info.insert(&asset_rights.usage_rights, &usage_tokens);

    // return value
    nft_666_token::NFT666Token{
        token_id,
        owner_id: asset_rights.ownership,
        usage_rights: asset_rights.usage_rights,
        metadata: Some(token_metadata),
    }
}