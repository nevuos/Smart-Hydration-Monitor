# Documentação do Projeto: Sistema Automatizado de Irrigação com Arduino

## Descrição
Este projeto utiliza um Arduino para monitorar e controlar a umidade do solo, empregando um sensor de umidade, um módulo Bluetooth para comunicação, e um sistema de controle automático para uma bomba de água. Ideal para projetos de jardinagem e agricultura inteligente.

## Componentes
- Arduino (Uno, Mega, etc.)
- Sensor de Umidade do Solo
- Módulo Bluetooth (HC-05)
- Relé
- Bomba de Água

## Funcionamento
- O sistema lê a umidade do solo e a envia em tempo real via Bluetooth.
- Controla automaticamente uma bomba de água com base na umidade detectada no solo.

## Instalação
1. Conecte o sensor de umidade do solo ao Arduino.
2. Conecte o módulo Bluetooth HC-05 nos pinos apropriados do Arduino.
3. Ligue o relé ao Arduino e à bomba de água.
4. Carregue o código no Arduino.
5. Utilize um monitor serial ou um app de terminal Bluetooth para ler os dados de umidade e controlar a bomba de água remotamente.

## Código
O código para este projeto foi otimizado com um Filtro de Kalman para leituras mais estáveis e contém funcionalidades de controle e monitoramento avançado. Ele pode ser baixado no link abaixo:
[Link para Download do Código](https://github.com/nevuos/Smart-Hydration-Monitor/archive/refs/heads/main.zip)

## Projeto Original e Esquema
Para mais informações, referências e esquemas de montagem, consulte o projeto original:
[Link para Projeto Original](https://circuitdigest.com/microcontroller-projects/automatic-irrigation-system-using-arduino-uno)

## Nota
- Assegure-se de que todas as conexões elétricas estejam seguras e isoladas.
- Lembre-se de desconectar os pinos RX e TX do módulo Bluetooth ao carregar o código no Arduino.
