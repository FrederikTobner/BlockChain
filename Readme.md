# Proof of Work
Simple toy blockchain, that illustrates the "proof of work" consens mechanism.
The Hashing function that is used for the blockchain is [sha-256](https://de.wikipedia.org/wiki/SHA-2).
The data in the blocks are not transactions, instead a sequence of characters is used as data.
The whole data sequence is hashed at once, so there are no [merkle trees](https://en.wikipedia.org/wiki/Merkle_tree) used in the blockchain.
Additionally the blockchain has no network capabilities and can only be used as a single node system, at the moment. 
