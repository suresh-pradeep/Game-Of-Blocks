# Smart Contract Based Loan Repayment
This is the implementation of a smart contract in which the owner initially has a balance of 100K MetaCoins acquired by taking loans. The debt the owner owes to a particular node is stored in a mapping called loans and is updated once a given node requests for the loan using ReqLoan function. The owner can view the dues that he/she has to a given node and choose to settle it.
## How the functions of the contract work
### Publically visible functions are:
- reqLoan(uint256 principle, uint rate, uint time): Arguments passsed are principal,rate and time as integers. Rate passed has to be an integer from 0 to 100. The function calls getCompundInterest() with the passed arguments and checks if the passed arguments are within the meaningful range(0-100 for rate,>0 for time,<100000 for principal). If they arguments are meaningful, it updates the debt in loans mapping,emits the event Request into the network and returns true. If not, it returns false and no updation is made.
- sendCoin(address receiver, uint256 amount, address sender): This is a function of the MetaCoin contract that implements the transfer of coins from the sender to receiver using balances mapping and emits the event Transfer into the network
- getBalance(address addr): This is a function of the MetaCoin contract that returns the balance of a given address in the network from the balances mapping.
- getOwnerBalance(): This is a function that returns the balance of the owner by calling the getBalance function with the owner's address that was initialised by the constructor
- getCompoundInterest(uint256 principle, uint rate, uint time): This is a function that calculates the compound interest given the principal,rate and time. It implements this using the 'while'loop with time as the control variable. As solidity doesn't provide good support for floating point numbers,the function implements the multiplication using quotients and reminders with respect to 100 as rate is passed as an integer percentage.
- viewDues(address ad): Function modifier IsOwner prevents a non-owner access to the function. When the owner calls this, it returns the debt that the owner owes to the passed address. The debt is the entry in the loans mapping corresponding to the passed address.
- settleDues(address ad): Function modifier IsOwner prevents a non-owner access to the function. When the owner calls this, it checks if the debt to the given address is initialised and non-zero. It then calls the sendCoin function of MetaCoin and checks if the return value is true. If true, it sets the pending loan for the address to zero. Otherwise, it returns false and the debt is not cleared.
### Internal functions:
- sendCoin(address receiver, uint256 amount, address sender): Internal function of MetaCoin which returns a bool. Checks if the sender has required balance in account and then transfers amount from sender's balance to receiver's balance in balances mapping. It then emits the event Tranfer into the network, specifying the sender, amount and receiver.
### Constructors of contracts:
- constructor of MetaCoin: It updates the entry corresponding to the owner's address to 100000 in the balances mapping
- constructor of Loan: It sets the Owner address to the node that deploys the contract. It further emits the event OwnerSet into the network
### Function modifiers:
- IsOwner: Checks if the calling node is the owner of the contract
## Instructions for testing and running Contract
- Copy the contents of assgn2.sol and paste it in a new file in Remix IDE online.
- Compile the file using Solidity compiler. Then use the deploy and run transactions menu.
- Deploy the contract from one account say acc1. Call getOwnerBalance() to confirm if the constructor has successfully initialised the value. It should return 100000.
- Now select a different account say acc2
    - call reqLoan from the account by passing appropriate parameters.Sample arguments: principal-19750, rate-5, time-2
    - The same arguments can be passed to getCompoundInterest function to display the dues that have been updated in the loans mapping. It should be 21773 for the sample arguments.
    - Copy the address of acc2 using the option near account
    - Pass the address to viewDues function and confirm that access is denied as given account is not Owner
    - Pass the same address to getBalance() function and confirm that the balances to all non-owner accounts is zero to start with.
    - Call getOwnerBalance to confirm access to function from other accounts. It should return 100000.
- Now select account as acc1
    - Pass address of acc2 that is already copied to viewDues function and make a call. It should return 21773 for the sample arguments.
    - Pass the same address to settleDues fucntion and make a call.
    - Pass the same address to getBalance function and make a call. It should return 21773 for the sample argument indicating that the loan debt has been transferred to the account
    - Pass the same address to viewDues function and make a call. It should return 0 indicating that dues have been cleared and the pending loan has been set to zero
    - Call getOwnerBalance function to confirm that the transferred amount has been debited from the owner balance. It should return 78227 for the sample arguements
- This template can be followed from multiple accounts by requesting loans and settling dues
- Further, following types of function calls can be made to test if the contract ignores/rejects such transactions
    - Principal,rate and time can be passed outside the above specified range to reqLoan
    - Dues can be viewed/settled for an address which hasn't requested debt amount using viewDues/settleDues
    - viewDues/settleDues can be called from non-owner account to check access
    - Owner can be requested to clear dues that amount to more than the owner balance i.e. settleDues will return false if debt in loans mapping is more than owner balance.
