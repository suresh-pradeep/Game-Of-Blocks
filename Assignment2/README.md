# Smart Contract Based Loan Repayment
This is the implementation of a smart contract in which the owner initially has a balance of 100K MetaCoins acquired by taking loans. The debt the owner owes to a particular node is stored in a mapping called loans and is updated once a given node requests for the loan using ReqLoan function. The owner can view the dues that he/she has to a given node and choose to settle it.
## Instructions to Run various functions of the contract and how they work
Publically visible functions are:
- reqLoan(uint256 principle, uint rate, uint time): Arguments passsed are principal,rate and time as integers. Rate passed has to be an integer from 0 to 100. The function calls getCompundInterest() with the passed arguments and checks if the passed arguments are within the meaningful range(0-100 for rate,>0 for time,<100000 for principal). If they arguments are meaningful, it updates the debt in loans mapping,emits the event Request into the network and returns true. If not, it returns false and no updation is made.
- sendCoin(address receiver, uint256 amount, address sender): This is a function of the MetaCoin contract that implements the transfer of coins from the sender to receiver using balances mapping and emits the event Transfer into the network
- getBalance(address addr): This is a function of the MetaCoin contract that returns the balance of a given address in the network from the balances mapping.
- getOwnerBalance(): This is a function that returns the balance of the owner by calling the getBalance function with the owner's address that was initialised by the constructor
- getCompoundInterest(uint256 principle, uint rate, uint time): This is a function that calculates the compound interest given the principal,rate and time. It implements this using the 'while'loop with time as the control variable. As solidity doesn't provide good support for floating point numbers,the function implements the multiplication using quotients and reminders with respect to 100 as rate is passed as an integer percentage.
- viewDues(address ad): Function modifier IsOwner prevents a non-owner access to the function. When the owner calls this, it returns the debt that the owner owes to the passed address. The debt is the entry in the loans mapping corresponding to the passed address.
- settleDues(address ad): Function modifier IsOwner prevents a non-owner access to the function. When the owner calls this, it checks if the debt to the given address is initialised and non-zero. It then calls the sendCoin function of MetaCoin and checks if the return value is true. If true, it sets the pending loan for the address to zero. Otherwise, it returns false and the debt is not cleared.
Constructors of contracts:
- constructor of MetaCoin: It updates the entry corresponding to the owner's address to 100000 in the balances mapping
- constructor of Loan: It sets the Owner address to the node that deploys the contract. It further emits the event OwnerSet into the network
Function modifiers:
-IsOwner: Checks if the calling node is the owner of the contract
## Instructions for Testing Contract
Copy the contents of assgn2.sol and paste it in Remix IDE online

