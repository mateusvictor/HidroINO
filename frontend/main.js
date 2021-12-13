// main.js
// Mateus Victor - 2021

const updateTime = 4000 // Intervalo a cada atualização do dashboard em ms
const prototypeId = "9d0efcbb-1c6f-4486-8066-19c9cb2352fd" // ID do protótipo HidroINO

// const url = 'http://localhost:8000/record-last/' + String(prototypeId) + '/'; // Rodando localmente
const url = 'https://hidroino.herokuapp.com/record-last/' + String(prototypeId) + '/' // Rodando com Heroku
let led1 = 0, sol1 = 0, sol2 = 0;
//const prototypeId = 1;
let counter = 1
let buttonsChanged = false;

let led1Btn = document.getElementById('led1')
let sol1Btn = document.getElementById('sol1')
let sol2Btn = document.getElementById('sol2')


let main = setInterval(requestAPI, updateTime) // A cada `updateTime` ms a função `requestAPI` é chamada. 	
function requestAPI(){
	// Realiza uma requisição do tipo GET e repassa os dados para a função `update`
	let response;
	fetch(url)
	    .then(res => res.json())
	    .then(data => response = data)
	    .then(() => update(response));
}

function update(response){
	// Seleciona e atualiza os valores presentes nos elementos `gauge`
	// Exemplo de `response`:
	// {
	// 	prototype_id: 1,
	// 	ph: 6.1,
	// 	temperature: 30.1,
	// 	humidity: 71,
	// 	datetime_creation: "11-09-2021 17:30:03",
	//  device1: true
	// }

	let phGauge = document.getElementById('ph').children;
	let tempGauge = document.getElementById('temp').children;
	let humidityGauge = document.getElementById('humidity').children;
	// Cada Gauge possui 3 "childrens": {div-percentage, div-mask, div-value}
	//
	// * div-percentage ou Gauge[0] é o elemento que abriga a barra de progresso:
	// 		- A partir da mudança do style desse elemento que ocorre a animação
	//
	// * div-value ou Gauge[2] é o elemento que possui a valor do medidor


	//led1 = response['device1'], sol1 = response['device2'], sol2 = response['device3']	

	tempPercentage = tempGauge[0], tempValue = tempGauge[2];
	phPercentage = phGauge[0], phValue = phGauge[2];
	humidityPercentage = humidityGauge[0], humidityValue = humidityGauge[2];

	tempValue.innerHTML = parseFloat(response['temperature']).toFixed(1) + ' ºC';
	phValue.innerHTML = response['ph'];
	humidityValue.innerHTML = response['humidity']+ "%";


	moveGauge(tempPercentage, response['temperature'] / 40)
	moveGauge(phPercentage, response['ph'] / 14)
	moveGauge(humidityPercentage, response['humidity'] / 100)

	console.log("Counter: ", counter)
	console.log(response)
	console.log('\n')

	updateDevice(led1Btn, response['device1'])
	updateDevice(sol1Btn, response['device2'])
	updateDevice(sol2Btn, response['device3'])
	
	counter += 1
}


function updateDevice(button, state){
	let text = String(button.innerText).split(' O')[0]
	if (state){
		text += ' ON'
		button.style['background-color'] = '#5cb85c'
	}
	else {
		text += ' OFF'
		button.style['background-color'] = '#F4654E'
	}
	button.innerHTML = text
}

function moveGauge(actualPercent, toMove){
	// Realiza a animação de crescimento/decrescimento da barra de progresso do medidor
	// actualPercent: porcentagem atual de preenchimento da barra da progresso
	// toMove: porcentagem desejada de preenchimento da barra de progresso

	let actualDeg = Math.floor(strToFloat(actualPercent.style['transform'])); // Angulação, em graus, da barra de progresso
	let newDeg = Math.floor(toMove * 180);

	let i = 0	
	if (i == 0){
		i = 1
		let change = setInterval(frame, 5); // Chama a função `frame` uma vez a cada 5 ms
		function frame(){
			// Incrementa/decrementa 1 grau a cada 5ms da rotação da barra de progresso do Gauge
			if (actualDeg == newDeg){
				i = 0;
				clearInterval(change);
			}
			else if (actualDeg < newDeg){
				actualDeg += 1
				actualPercent.style['transform'] = 'rotate(' + String(actualDeg) + 'deg)'
			}
			else if (actualDeg > newDeg){
				actualDeg -= 1
				actualPercent.style['transform'] = 'rotate(' + String(actualDeg) + 'deg)'
			}
		}
	}
	actualPercent.style['transform'] = 'rotate(' + String(newDeg) + 'deg)';
}


function strToFloat(degrees){
	return degrees.slice(7, degrees.indexOf('d'))
}