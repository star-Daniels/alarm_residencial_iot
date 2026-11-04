import { initializeApp } from "https://www.gstatic.com/firebasejs/10.12.2/firebase-app.js";
import { getDatabase, ref, onValue, set } from "https://www.gstatic.com/firebasejs/10.12.2/firebase-database.js";

const firebaseConfig = {
  apiKey: "AIzaSyApRkEsG514s5kI1W3E2Wiph15a3-KCGrc",
  authDomain: "alarme-c1a35.firebaseapp.com",
  databaseURL: "https://alarme-c1a35-default-rtdb.firebaseio.com",
  projectId: "alarme-c1a35",
  storageBucket: "alarme-c1a35.firebasestorage.app",
  messagingSenderId: "839051066466",
  appId: "1:839051066466:web:e35f4c5d5a524fe972a53b"
};

const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

const ativoRef = ref(db, "sistema/ativo");
const movimentoRef = ref(db, "sistema/movimento");


const conexao = document.getElementById("conn");
const conexaoTexto = document.getElementById("connText");
const estado = document.getElementById("armState");
const botaoArmar = document.getElementById("armToggle");
const caixaMovimento = document.getElementById("motionBox");
const tituloMovimento = document.getElementById("motionTitle");
const textoMovimento = document.getElementById("motionSub");
const botaoLimpar = document.getElementById("clearBtn");
const lista = document.getElementById("logList");


function registrar(texto, alerta) {
  if (lista.querySelector(".empty")) lista.innerHTML = "";

  const item = document.createElement("li");
  if (alerta) item.className = "mov";
  item.innerHTML = `<span>${texto}</span><time>${new Date().toLocaleTimeString("pt-BR")}</time>`;
  lista.prepend(item);
}

function mostrarErro(erro) {
  console.error(erro);
  conexao.className = "conn off";
  conexaoTexto.textContent = "Erro: " + erro.code;
}


onValue(ref(db, ".info/connected"), (snap) => {
  const online = snap.val() === true;
  conexao.className = online ? "conn on" : "conn off";
  conexaoTexto.textContent = online ? "Online" : "Sem conexão";
});


onValue(ativoRef, (snap) => {
  const armado = snap.val() === true;

  estado.textContent = armado ? "Armado" : "Desarmado";
  estado.className = armado ? "state armed" : "state disarmed";
  botaoArmar.checked = armado;
  botaoArmar.disabled = false;

  registrar(armado ? "Sistema armado" : "Sistema desarmado");
}, mostrarErro);


botaoArmar.addEventListener("change", () => {
  set(ativoRef, botaoArmar.checked).catch(mostrarErro);
});


onValue(movimentoRef, (snap) => {
  const movimento = snap.val() === true;

  caixaMovimento.classList.toggle("alert", movimento);
  botaoLimpar.disabled = !movimento;

  if (movimento) {
    tituloMovimento.textContent = "Movimento detectado";
    textoMovimento.textContent = "Detectado às " + new Date().toLocaleTimeString("pt-BR");
    registrar("Movimento detectado", true);
  } else {
    tituloMovimento.textContent = "Sem movimento";
    textoMovimento.textContent = "Nenhuma movimentação registrada.";
  }
}, mostrarErro);


botaoLimpar.addEventListener("click", () => {
  set(movimentoRef, false).catch(mostrarErro);
  registrar("Alerta limpo");
});
