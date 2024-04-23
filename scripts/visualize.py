import matplotlib.pyplot as plt
f = open('../UAV/example.txt','r')
data = f.readlines()
a = list()
b = list()
c = list()
d = list()
for x in data:
    a1,b1,c1,d1 = map(float,x.split(' '))
    a.append(a1)
    b.append(b1)
    c.append(c1)
    d.append(d1)

f.close()
plt.plot(a,color='r',label='angular_position_x')
plt.plot(b,color='g',label='angular_position_y')
plt.plot(c,color='b',label='angular_position_z')
plt.plot(d,color='black',label='PID x')
plt.xlabel("Time")
plt.ylabel("position ")
plt.title("Gyro Quaternion")
plt.legend()
plt.show()
