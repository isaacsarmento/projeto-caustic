const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");

// Elements for sensor readings
const humElement = document.getElementById("hum");
const statusMensagemElement = document.getElementById("statusMensagem");
const statusBuzzerButton = document.querySelector('#statusBuzzerElement');
const statusBuzzerRef = firebase.database().ref().child('/statusBuzzer');
statusBuzzerRef.set(1);


// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display = 'block';
    authBarElement.style.display ='block';
    userDetailsElement.style.display ='block';
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from database
    var uid = user.uid;
    console.log(uid);

    // Database paths (with user UID)
    var dbPathHum = '/statusSensor';
    var statusMensagemPath = '/statusMensagem';
   

    // Database references
    var dbRefHum = firebase.database().ref().child(dbPathHum);
    //var statusRefMensagem = firebase.database().ref().child(statusMensagemPath);
    

    // Update page with new readings
    dbRefHum.on('child_added', (snapshot) => {
      const data = snapshot.val();
      if (data !== null) {
        humElement.innerText = data.value.toFixed(2);
      } else {
        console.log('No data found at /statusSensor');
      }
    });

    statusBuzzerButton.addEventListener('click', () => {
      // Leia o valor atual de /statusBuzzer
      statusBuzzerRef.once('value').then((snapshot) => {
        const currentValue = snapshot.val();
        // Alterne o valor entre 1 e 0
        const newValue = currentValue === 1 ? 0 : 1;
        // Atualize o valor no banco de dados
        statusBuzzerRef.set(newValue);
        // Exiba um alerta com base no novo valor
        if (newValue === 0) {
          alert('O alarme sonoro foi desativado!');
        } else if (newValue === 1) {
          alert('O alarme sonoro foi ativado!');
        }
      });
    });

  // if user is logged out
  } else{
    // toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display ='none';
    userDetailsElement.style.display ='none';
    contentElement.style.display = 'none';
  }
}