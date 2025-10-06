const aedes = require("aedes")();
const mqtt = require("mqtt");
const { createServer } = require("net");
const mysql = require("mysql2");

//Conexão Mysql
const pool = mysql.createPool({
  host: "192.168.245.81",
  user: "alunods",
  password: "senai@604",
  database: "mqtt",
});

// Configuração do Broker Mqtt
const server = createServer(aedes.handle);
const PORT = 1883;
server.listen(PORT, () => {
  console.log(`Broker Mqtt rodando na porta ${PORT}`);
});

aedes.on("publish", async (packet, client) => {
  if (packet.topic === "iot/esp32") {
    try {
      const payload = JSON.parse(packet.payload.toString());

      //Exemplo de payload
      // {sensor: "temperatura", valor:25.5}
      const { sensor, valor } = payload;

      //Inserir no Mysql
      pool.query(
        `INSERT INTO sensores(sensor,valor) VALUES (?,?)`,
        [sensor, valor],
        (err, results) => {
            if(err) console.error("Error ao inserir no mysql: ", err)
            else console.log("Dados inseridos: ",results);
        }
      );
    } catch (error) {
        console.error("Erro ao processar mensagem: ", error)
    }
  }
});
