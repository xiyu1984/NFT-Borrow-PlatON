#include "NFT666.hpp"
#include <platon/panic.hpp>

void NFT666::init()
{

}

void NFT666::lendUsageTo(const std::string& to, const std::string& token_id, uint64_t period)
{
    auto toAddress = platon::make_address(to);
    if (!toAddress.second)
    {
        platon::internal::platon_throw("Invalid `to` account!");
    }
    
    platon::Address pre_account = platon::platon_caller();

    if (!owner_ship.contains(token_id))
    {
        platon::internal::platon_throw("token does not exist!");
    }
    auto art = &owner_ship[token_id];
    if (!((art->ownership == pre_account) && (art->usage_rights == pre_account)))
    {
        platon::internal::platon_throw("Lend Unauthorized!");
    }
    
    if (!assets_usage_info.contains(art->usage_rights))
    {
         platon::internal::platon_throw("There's a bug, because someone has the usage_right, but the asset does not existed in the asset_usage_info table!");
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
    return platon::u128(0);
}

std::string NFT666::ownerOf(const std::string& _tokenId)
{
    return std::string("");
}

void NFT666::safeTransferFromWithData(const std::string& _from, const std::string& _to, const std::string& _tokenId, platon::bytes data)
{

}

void NFT666::safeTransferFrom(const std::string& _from, const std::string& _to, const std::string& _tokenId)
{

}

void NFT666::transferFrom(const std::string& _from, const std::string& _to, const std::string& _tokenId)
{

}

void NFT666::approve(const std::string& _approved, const std::string& _tokenId)
{

}

void NFT666::setApprovalForAll(const std::string& _operator, bool _approved)
{

}

std::string NFT666::getApproved(const std::string& _tokenId)
{
    return std::string("");
}

bool NFT666::isApprovedForAll(const std::string& _owner, const std::string& _operator)
{
    return true;
}

uint64_t NFT666::totalSupply()
{
    return uint64_t(0);
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
    return std::string("");
}

void NFT666::transfer_usage_without_check(const std::string& from, const std::string& to, const std::string& token_id)
{

}

void NFT666::transfer_ownership_without_check(const std::string& from, const std::string& to, const std::string& token_id)
{
    
}