// Obtém o elemento pelo ID
let gasLevelElement = document.getElementById("hum");
let realtimeElement = document.getElementById("mensagemReal")

console.log("Antes do if");

// Verifica o valor do sensor
if (gasLevelElement.innerHTML >= 900) {
  // Atualiza o conteúdo da página com a mensagem desejada
  realtimeElement.innerHTML = "Nível Crítico: O nível crítico de gás pode ser considerado acima de 6000. Nessa faixa, a concentração de gás atinge um nível perigoso, representando um alto risco de explosão, incêndio ou intoxicação grave. Nessa etapa acontece uma recalibração no dispositivo. Caso não for uma alarme falso, acionaremos novamente o sistema de alerta ao usuário e o fechamento da válvula. Se possível, recomendamos evacuar o ambiente e acionar as autoridades competentes para garantir a segurança de todos.";
}
console.log("depois do if");
console.log("Valor do sensor:", gasLevelElement.innerHTML);