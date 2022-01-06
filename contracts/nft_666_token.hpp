#pragma once

#include "MetaData.hpp"

struct NFT666Token
{
    std::string                     token_id;
    platon::Address                 owner_id;
    platon::Address                 usage_rights;
    std::vector<TokenMetaData>      metadata;

    PLATON_SERIALIZE(NFT666Token, (token_id)(owner_id)(usage_rights)(metadata))      
};

