import hashlib
import time
nonce=1
message=input("Enter the message: ")
target="00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
string=message+str(nonce)
m=hashlib.sha256(string.encode())
res=bytes(target,'utf-8')
start_time=time.time()
while bytes(m.hexdigest(),'utf-8')>res:
    nonce+=1
    string=message+str(nonce)
    m=hashlib.sha256(string.encode())
end_time=time.time()
print(nonce,"\n"+str(end_time-start_time)+" secs")
