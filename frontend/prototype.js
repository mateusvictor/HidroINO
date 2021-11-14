async function submitHandler(event){
	let name = document.getElementById('name')
	let prototypeID = document.getElementById('prototypeID')

	console.log(name.value)

	let response = await requestAPI({owner_name: name.value})
	console.log(response)

	prototypeID.innerHTML = response.id

}


const btn = document.getElementById('submit')
btn.addEventListener('click', submitHandler)

async function requestAPI(data){
	let response = await fetch('http://localhost:8000/prototypes/', {
		method: 'post',
		headers: {
		'Accept': 'application/json, text/plain, */*',
		'Content-Type': 'application/json'
		},
		body: JSON.stringify(data)
	})
	.then(res => res.json());
//	.then(res => console.log(res));
	
	return response
}	