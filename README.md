# Alarme com ESP32 e Firebase

Painel web para controlar um sistema de alarme feito com ESP32. O ESP32 detecta movimento e envia os dados para o Firebase Realtime Database. A página mostra tudo em tempo real e permite armar ou desarmar o sistema e limpar o alerta de movimento.

## Como funciona

O ESP32 lê o campo sistema/ativo para saber se o alarme está armado.
Se estiver armado e o sensor detectar movimento, ele grava true em sistema/movimento.
O site recebe a mudança na hora e mostra o alerta em vermelho.
Quem decide quando o alerta acaba é o site: o botão Limpar alerta grava false em sistema/movimento. O ESP32 nunca volta esse valor para false sozinho.
Um bot do Telegram envia as mensagens para o grupo do projeto.

## Funcionalidades
* Mostra se o sistema está armado ou desarmado.
* Botão deslizante para armar e desarmar.
* Alerta visual quando há movimento, com o horário da detecção.
* Botão para limpar o alerta (só o site faz isso).
* Indicador de conexão com o Firebase.
* Histórico de eventos da sessão atual.
* Link para entrar no grupo do Telegram.
* Tema escuro em roxo, responsivo para celular.
