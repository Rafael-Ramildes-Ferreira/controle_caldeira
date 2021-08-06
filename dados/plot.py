import matplotlib.pyplot as plt

file = open("./dados.csv")


temp_desej = []
temp_efetiv = []
temp_atuac = []
nivel_desej = []
nivel_efetiv = []
nivel_atuac = []

next(file)
for linha in file:
	try:
		elementos = linha.split(",")
		temp_desej.append( float(elementos[0]) )
		temp_efetiv.append( float(elementos[1]) )
		#temp_atuac.append( float(elementos[2]) )
		nivel_desej.append( float(elementos[3]) )
		nivel_efetiv.append( float(elementos[4]) )
		#nivel_atuac.append( float(elementos[5]) )
	except: pass

fig, axs = plt.subplots(2,sharex=True)

X = [x for x in range(len(temp_desej)-1)]
axs[0].plot(X,temp_desej[1:],label="Temperatura desejada")
axs[0].plot(X,temp_efetiv[1:],label="Temperatura efetiva")
#plt.plot(temp_atuac[1:],label="Sinal de atuação do aquecedor (*10^4)")
axs[1].plot(X,nivel_desej[1:],label="Nível desejado")
axs[1].plot(X,nivel_efetiv[1:],label="Nível efetivo")
#plt.plot(nivel_atuac[1:],label="Sinal de atuação da entrada d'água")

axs[0].set_ylabel("Celsius")
axs[1].set_ylabel("Metros")
axs[1].set_xlabel("Segundos")

axs[0].legend()
axs[1].legend()

plt.subplots_adjust(hspace=0)

plt.legend()
plt.show()

