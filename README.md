# Projeto HidroINO

Repositório contendo a aplicação web do projeto HidroINO e os códigos utilizados no hardware Arduino Uno e no módulo ESP8266.

## Sobre

O projeto HidroINO consiste em um sistema de hidropônico de plantação de baixo custo por técnica de filme de nutrientes, que se baseia na cultura de plantas sem a utilização do solo, ideal para o cultivo de hortaliças de folhas como o alface.

O sistema conta com sensores para o monitoramento do ambiente, cujos dados e informações são disponibilizados em uma interface web que pode ser acessada por qualquer dispositivo que tenha conexão à internet.

 
<!--
## Instalação
- Realize o download do repositório com o git

```bash
git clone https://github.com/mateusvictor/HidroINO.git
```

- Crie um ambiente virtual no diretório do projeto

```bash
python -m venv venv
```

- Ative o ambiente virtual

```bash
venv\Scripts\activate.bat
```

- Instale as dependencias do projeto

```bash
pip install -r requirements.txt
```
--> 

## Funcionamento

De uma forma simples e intuiva, o diagrama abaixo mostra a visão geral da aplicação:

<img src="https://github.com/mateusvictor/HidroINO/blob/main/screenshots/geral.jpg">

Inicialmente, o código presente no hardware Arduino envia uma requisição HTTP do tipo POST para o servidor no endpoint ```/records/```. O corpo da respectiva requisição contém as informações para criação de um novo registro da tabela ```records```.

Após a validação e inscrição do novo registro no banco de dados, as informações recém criadas se tornam disponíveis no endpoint ```/record-last/{prototype_id}/```, que, por sua vez, possibilita a leitura do último registro informado para um certo protótipo, através de uma requisição HTTP do tipo GET, pelo front-end da aplicação, criado na linguagem JavaScript.

Dashboard da aplicação:

<img src="https://github.com/mateusvictor/HidroINO/blob/main/screenshots/dashboard.jpg">

Documentação da API gerada automaticamente com Swagger:

<img src="https://github.com/mateusvictor/HidroINO/blob/main/screenshots/swagger-ui.jpg">

## Outros detalhes

* Serviço de hospedagem da API: <a href="https://www.heroku.com/">Heroku</a>
	* Link da documentação com Swagger: <a href="https://hidroino.herokuapp.com/docs">hidroino.herokuapp.com/docs</a>

* Não foi realizada a hospedagem do front-end da aplicação, mas uma idéia a ser considerada futuramente é a hospedagem com o <a href="https://pages.github.com/">GitHub Pages</a>

* Link da monografia do TCC com informações sobre os sensores utilizados, esquema do circuito principal, objetivos, dentre outras: <a href="">Em breve!</a>

### Skills adquiridas durante o desenvolvimento do projeto
* Criação de APIs utilizando o framework FastAPI;
* Consumo de APIs e interação dos dados com a página principal na linguagem JavaScript;
* Programação do hardware Arduino e utilização do módulo ESP8266 para o envio dos dados obtidos para o servidor.
