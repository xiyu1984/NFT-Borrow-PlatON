#pragma once

#include "nft_666_token.hpp"

struct EventTransfer{
    std::string     from;
    std::string     to;
    std::string     tokenId;

    PLATON_SERIALIZE(EventTransfer, (from)(to)(tokenId))
};

struct EventApproval{
    std::string     owner;
    std::string     approved;
    std::string     tokenId;

    PLATON_SERIALIZE(EventApproval, (owner)(approved)(tokenId))
};

struct EventApprovalForAll{
    std::string     owner;
    std::string     _operator;
    bool            approved;

    PLATON_SERIALIZE(EventApprovalForAll, (owner)(_operator)(approved))
};

class NFT666: public platon::Contract{

public:
    ACTION void init();

    // NFT interfaces
    //////////////////////////////////////////////////////////////////////////////////////////// PRC-666 interfaces
    /// @notice The owner lend his token to someone else, the `predecessor_account_id` must be the token owner.
    /// The `usage_rights` must belong to the owner.
    /// @dev 
    /// @param to The address of the next user
    /// @param token_id The ID of the token
    /// @param period The rental period of the leasing. The owner can `get_usage_back` after `env::block_height() + period`
    ACTION void lendUsageTo(const std::string& to, const std::string& token_id, uint64_t period);

    /// @notice The borrower returns the token to the owner, the `predecessor_account_id` must be the token user.
    /// @dev 
    /// @param token_id The ID of the token
    ACTION void usageReturn(const std::string& token_id);

    /// @notice Get the leasing period of token_id.
    /// @dev 
    /// @param token_id The ID of the token
    /// @returns time up block_height
    CONST uint64_t getLeasingPeriod(const std::string& token_id);

private:
    void transfer_usage_without_check(const std::string& from, const std::string& to, const std::string& token_id);

    void transfer_ownership_without_check(const std::string& from, const std::string& to, const std::string& token_id);

public:
    //////////////////////////////////////////////////////////////////////////////////////////// Derived from ERC-721
    /// @dev This emits when ownership of any NFT changes by any mechanism.
    ///  This event emits when NFTs are created (`from` == 0) and destroyed
    ///  (`to` == 0). Exception: during contract creation, any number of NFTs
    ///  may be created and assigned without emitting Transfer. At the time of
    ///  any transfer, the approved address for that NFT (if any) is reset to none.
    PLATON_EVENT1(TransferEvent, std::string, EventTransfer);

    /// @dev This emits when the approved address for an NFT is changed or
    ///  reaffirmed. The zero address indicates there is no approved address.
    ///  When a Transfer event emits, this also indicates that the approved
    ///  address for that NFT (if any) is reset to none.
    PLATON_EVENT1(ApprovalEvent, std::string, EventApproval);

    /// @dev This emits when an operator is enabled or disabled for an owner.
    ///  The operator can manage all NFTs of the owner.
    PLATON_EVENT1(ApprovalForAllEvent, std::string, EventApprovalForAll);

    /// @notice Count all NFTs assigned to an owner
    /// @dev NFTs assigned to the zero address are considered invalid, and this
    ///  function throws for queries about the zero address.
    /// @param _owner An address for whom to query the balance
    /// @return The number of NFTs owned by `_owner`, possibly zero
    CONST platon::u128 balanceOf(const std::string& _owner);

    /// @notice Find the owner of an NFT
    /// @dev NFTs assigned to zero address are considered invalid, and queries
    ///  about them do throw.
    /// @param _tokenId The identifier for an NFT
    /// @return The address of the owner of the NFT
    CONST std::string ownerOf(const std::string& _tokenId);

    /// @notice Transfers the ownership of an NFT from one address to another address
    /// @dev Throws unless `msg.sender` is the current owner, an authorized
    ///  operator, or the approved address for this NFT. Throws if `_from` is
    ///  not the current owner. Throws if `_to` is the zero address. Throws if
    ///  `_tokenId` is not a valid NFT. When transfer is complete, this function
    ///  checks if `_to` is a smart contract (code size > 0). If so, it calls
    ///  `onERC721Received` on `_to` and throws if the return value is not
    ///  `bytes4(keccak256("onERC721Received(address,address,uint256,bytes)"))`.
    /// @param _from The current owner of the NFT
    /// @param _to The new owner
    /// @param _tokenId The NFT to transfer
    /// @param data Additional data with no specified format, sent in call to `_to`
    ACTION void safeTransferFromWithData(const std::string& _from, const std::string& _to, const std::string& _tokenId, platon::bytes data);

    /// @notice Transfers the ownership of an NFT from one address to another address
    /// @dev This works identically to the other function with an extra data parameter,
    ///  except this function just sets data to "".
    /// @param _from The current owner of the NFT
    /// @param _to The new owner
    /// @param _tokenId The NFT to transfer
    ACTION void safeTransferFrom(const std::string& _from, const std::string& _to, const std::string& _tokenId);

    /// @notice Transfer ownership of an NFT -- THE CALLER IS RESPONSIBLE
    ///  TO CONFIRM THAT `_to` IS CAPABLE OF RECEIVING NFTS OR ELSE
    ///  THEY MAY BE PERMANENTLY LOST
    /// @dev Throws unless `msg.sender` is the current owner, an authorized
    ///  operator, or the approved address for this NFT. Throws if `_from` is
    ///  not the current owner. Throws if `_to` is the zero address. Throws if
    ///  `_tokenId` is not a valid NFT.
    /// @param _from The current owner of the NFT
    /// @param _to The new owner
    /// @param _tokenId The NFT to transfer
    ACTION void transferFrom(const std::string& _from, const std::string& _to, const std::string& _tokenId);

    /// @notice Change or reaffirm the approved address for an NFT
    /// @dev The zero address indicates there is no approved address.
    ///  Throws unless `msg.sender` is the current NFT owner, or an authorized
    ///  operator of the current owner.
    /// @param _approved The new approved NFT controller
    /// @param _tokenId The NFT to approve
    ACTION void approve(const std::string& _approved, const std::string& _tokenId);

    /// @notice Enable or disable approval for a third party ("operator") to manage
    ///  all of `msg.sender`'s assets
    /// @dev Emits the ApprovalForAll event. The contract MUST allow
    ///  multiple operators per owner.
    /// @param _operator Address to add to the set of authorized operators
    /// @param _approved True if the operator is approved, false to revoke approval
    ACTION void setApprovalForAll(const std::string& _operator, bool _approved);

    /// @notice Get the approved address for a single NFT
    /// @dev Throws if `_tokenId` is not a valid NFT.
    /// @param _tokenId The NFT to find the approved address for
    /// @return The approved address for this NFT, or the zero address if there is none
    CONST std::string getApproved(const std::string& _tokenId);

    /// @notice Query if an address is an authorized operator for another address
    /// @param _owner The address that owns the NFTs
    /// @param _operator The address that acts on behalf of the owner
    /// @return True if `_operator` is an approved operator for `_owner`, false otherwise
    CONST bool isApprovedForAll(const std::string& _owner, const std::string& _operator);

    //////////////////////////////////////////////////////////////////////////////////////////// Derived from ERC721Enumerable 
    /// @notice Count NFTs tracked by this contract
    /// @return A count of valid NFTs tracked by this contract, where each one of
    ///  them has an assigned and queryable owner not equal to the zero address
    CONST uint64_t totalSupply();

    //////////////////////////////////////////////////////////////////////////////////////////// Derived from ERC721Metadata
    /// @notice A descriptive name for a collection of NFTs in this contract
    CONST std::string name();

    /// @notice An abbreviated name for NFTs in this contract
    CONST std::string symbol();

    /// @notice A distinct Uniform Resource Identifier (URI) for a given asset.
    /// @dev Throws if `_tokenId` is not a valid NFT. URIs are defined in RFC
    ///  3986. The URI may point to a JSON file that conforms to the "ERC721
    ///  Metadata JSON Schema".
    CONST std::string tokenURI(const std::string& _tokenId); 

private:
    platon::StorageType<"Owner"_n, std::pair<platon::Address, bool>>    owner_id;
    platon::StorageType<"total"_n, uint64_t>                            total_supply;
    platon::StorageType<"Contract"_n, ContractMetaData>                 contract_meta;
    
};

PLATON_DISPATCH(NFT666, (init)
(lendUsageTo)(usageReturn)(getLeasingPeriod)
(balanceOf)(ownerOf)
(safeTransferFromWithData)(safeTransferFrom)
(transferFrom)(approve)(setApprovalForAll)(getApproved)(isApprovedForAll)
(totalSupply)(name)(symbol)(tokenURI))