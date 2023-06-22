const myImage = document.querySelector("img");

myImage.onclick = () => {
  const mySrc = myImage.getAttribute("src");
  if (mySrc === "images/frontEndImage.png") {
    myImage.setAttribute("src", "images/logo.png");
  } else {
    myImage.setAttribute("src", "images/frontEndImage.png");
  }
};

let myButton = document.querySelector("button");
let myHeading = document.querySelector("h1");

function setUserName() {
    const myName = prompt("Please enter your name.");
    if (!myName)
        setUserName();
    localStorage.setItem("name", myName);
    myHeading.textContent = `you are in my portfolyo web page, ${myName}`;
  }

  if (!localStorage.getItem("name")) {
    setUserName();
  } else {
    const storedName = localStorage.getItem("name");
    myHeading.textContent = `you are in my portfolyo web page, ${storedName}`;
  }

  myButton.onclick = () => {
    setUserName();
  };