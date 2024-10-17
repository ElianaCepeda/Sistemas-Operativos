# Cliente
Descripción: 
En este programa se implementa un cliente, el cual se comunica de manera bi-direccional con el Servidor
a través de un PipeName llamado FIFO_FILE, el cual se crea temporalmente en "/tmp/fifo_twoway"
El cliente envía mensajes al servidor, recibiendo una respuesta del servidor con el mensaje escrito al revés.
La comunicación finaliza cuando el cliente envia el mensaje "end"

# Servidor
Descripción: 
En este programa se implementa un servidor, el cual se comunica de manera bi-direccional con el cliente
a través de un PipeName llamado FIFO_FILE, el cual se crea temporalmente en "/tmp/fifo_twoway"
El servidor recibe mensajes del cliente, y envía una respuesta con el mensaje al revés, esto lo hace infinitamente
hasta que recibe el mensaje "end" proveniente del cliente
