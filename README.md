# projeto1-fse

Trabalho 1 da disciplina de Fundamentos de Sistemas Embarcados (2022/1). Controle de semáforo com socket

- Nome: Emily Dias Sousa
- Matrícula: 18/0015966

## Instalação

- Deve ser clonado o repositório

- Após clonar o repositório, entre na pasta do servidor central:

```bash
cd centralServer
```

- Em outro terminal, entre na pasta do servidor central:

```bash
cd distributedServer
```

## Execução

Para executar, é necessário executar o servidor central primeiro, com mais duas instâncias do servidor distribuído, seguindo os comandos:

- centralServer

```bash
make
bin/run
```

- distributedServer (abra um terminal para cada placa)

```bash
make
bin/run placa1
```

```bash
make
bin/run placa2
```

placa1 é o GPIO Cruzamento 1 e a placa2 é o GPIO Cruzamento 2 do trabalho: https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-1-2022-1 .

## Modo de Uso

O servidor central aceita até 4 conexões simultâneas, sendo os 4 cruzamentos. O teste que foi feito executando RASP 44 - servidor central com as placas do servidor distribuído (as duas instâncias) e com a RASP 43 - 2 instâncias do servidor distribuído.