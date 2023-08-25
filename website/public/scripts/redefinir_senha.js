var emailInput = document.getElementById("email");
var resetPasswordButton = document.getElementById("reset-password-button");
var messageElement = document.getElementById("message");

// Adiciona um evento de clique ao botão de redefinição de senha
resetPasswordButton.addEventListener("click", function() {
  // Obtém o endereço de email do usuário
  var email = emailInput.value;

  // Envia o email de redefinição de senha
  firebase.auth().sendPasswordResetEmail(email).then(function() {
    console.log("Dentro da função then");
    // Email enviado com sucesso
    messageElement.innerHTML = "Email de redefinição de senha enviado!";
  }).catch(function(error) {
    // Ocorreu um erro ao enviar o email
    messageElement.innerHTML = "Erro ao enviar o email de redefinição de senha: " + error.message;
    console.log("Erro ao enviar o email de redefinição de senha:", error);
  });
});