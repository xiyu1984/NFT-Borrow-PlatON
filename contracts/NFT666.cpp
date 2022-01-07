#include "NFT666.hpp"

void NFT666::init()
{

}

void NFT666::lendUsageTo(const std::string& to, const std::string& token_id, uint64_t period)
{
    
}

void NFT666::usageReturn(const std::string& token_id)
{

}

uint64_t NFT666::getLeasingPeriod(const std::string& token_id)
{
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