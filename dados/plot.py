import matplotlib.pyplot as plt

file = open("./dados.csv")


tempo = []
temp_desej = []
temp_efetiv = []
nivel_desej = []
nivel_efetiv = []

next(file)
for linha in file:
	try:
		elementos = linha.split(",")
		min_sec = elementos[0].split(":")
		tempo.append( int(min_sec[0])*60 + float(min_sec[1]) )
		temp_desej.append( float(elementos[1]) )
		temp_efetiv.append( float(elementos[2]) )
		nivel_desej.append( float(elementos[3]) )
		nivel_efetiv.append( float(elementos[4]) )
	except: pass

fig, axs = plt.subplots(2,sharex=True)

axs[0].plot(tempo,temp_desej,label="Temperatura desejada")
axs[0].plot(tempo,temp_efetiv,label="Temperatura efetiva")
axs[1].plot(tempo,nivel_desej,label="Nível desejado")
axs[1].plot(tempo,nivel_efetiv,label="Nível efetivo")

axs[0].set_ylabel("Celsius")
axs[1].set_ylabel("Metros")
axs[1].set_xlabel("Segundos")

axs[0].legend()
axs[1].legend()

plt.subplots_adjust(hspace=0)

plt.legend()
plt.show()

