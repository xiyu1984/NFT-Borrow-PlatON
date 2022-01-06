#pragma once

#include <platon/platon.hpp>

#include <string>
#include <vector>

struct ContractMetaData
{
    std::string                     spec;           // required, essentially a version like "nft-1.0.0"
    std::string                     name;           // required, ex. "platon-nft"
    std::string                     symbol;         // required, ex. "PlatON to the moon"
    std::vector<std::string>        icon;           // Data URL
    std::vector<std::string>        base_uri;       // Centralized gateway known to have reliable access to decentralized storage assets referenced by `reference` or `media` URLs
    std::vector<std::string>        reference;      // URL to a JSON file with more info
    std::vector<std::string>        reference_hash; // Base64-encoded sha256 hash of JSON from reference field. Required if `reference` is included.

    PLATON_SERIALIZE(ContractMetaData, (spec)(name)(symbol)(icon)(base_uri)(reference)(reference_hash))
};

struct TokenMetaData {
    std::vector<std::string>        title;           // ex. "Arch Nemesis: Mail Carrier" or "Parcel #5055"
    std::vector<std::string>        description;     // free-form description
    std::vector<std::string>        media;           // URL to associated media, preferably to decentralized, content-addressed storage
    std::vector<platon::bytes>      media_hash;      // Base64-encoded sha256 hash of content referenced by the `media` field. Required if `media` is included.
    uint64_t                        copies;          // number of copies of this set of metadata in existence when token was minted.
    std::vector<std::string>        issued_at;       // ISO 8601 datetime when token was issued or minted
    std::vector<std::string>        expires_at;      // ISO 8601 datetime when token expires
    std::vector<std::string>        starts_at;       // ISO 8601 datetime when token starts being valid
    std::vector<std::string>        updated_at;      // ISO 8601 datetime when token was last updated
    std::vector<std::string>        extra;           // anything extra the NFT wants to store on-chain. Can be stringified JSON.
    std::vector<std::string>        reference;       // URL to an off-chain JSON file with more info.
    std::vector<platon::bytes>      reference_hash;  // Base64-encoded sha256 hash of JSON from reference field. Required if `reference` is included.

    PLATON_SERIALIZE(TokenMetaData, (title)(description)(media)(media_hash)
    (copies)(issued_at)(expires_at)(starts_at)
    (updated_at)(extra)(reference)(reference_hash))
};
