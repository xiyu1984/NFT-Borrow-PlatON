#pragma once

#include "nft_666_token.hpp"

class NFT666: public platon::Contract{

public:
    ACTION void init();

    // NFT interfaces

private:
    platon::StorageType<"Owner"_n, std::pair<platon::Address, bool>>    owner_id;
    platon::StorageType<"total"_n, uint64_t>                            total_supply;
    platon::StorageType<"Contract"_n, ContractMetaData>                 contract_meta;
    
};

PLATON_DISPATCH(NFT666, (init))