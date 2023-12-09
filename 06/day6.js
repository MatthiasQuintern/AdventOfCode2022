let output = document.getElementById("output");
let input;
let fileContent;

function readFile(input) {
    let file = input.files[0]; 
    console.log("Reading file", file)
    let fileReader = new FileReader();
    fileReader.readAsText(file); 
    fileReader.onload = function() {
        fileContent = fileReader.result;
  }; 
  fileReader.onerror = function() {
      alert(fileReader.error);
  }; 
}

let start = document.getElementById("start");
start.addEventListener("mouseover", () => {
    start.innerHTML = "JUST DO IT"
});
start.addEventListener("mouseout", () => {
    start.innerHTML = "Read File"
});
start.addEventListener("click", () => {
    findPosition(fileContent)
});


function findPosition(line) {
    console.time("day06")
    let i = 4;
    let packetStart = line.substring(0, 4);  // should always contain 4 chars
    let messageStart = line.substring(0, 14);  // should always contain 14 chars
    let packetStartI = 0;
    while (i < line.length) {
        console.log(i, "Char", line[i], "chars:", packetStart);
        // if packetStart not found, look for packet start
        if (packetStartI == 0) {
            if (/(.).*\1/.test(packetStart)) {   // if unique
                packetStart = packetStart.substring(1, 4) + line[i];
            }
            else {
                packetStartI = i;
                messageStart = line.substring(i, i + 14);
            }
        }
        else {  // search for message start
            if (/(.).*\1/.test(messageStart)) {   // if unique
                messageStart = messageStart.substring(1, 14) + line[i];
            }
            else {
                output.innerHTML = "Found the first packet start marker at position <b>" + packetStartI + "</b>, the marker is " + packetStart + "<br>" +
                    "Found the first message start marker at position <b>" + i + "</b>, the marker is " + messageStart + "\n";
                console.timeEnd("day06")
              return;
            }
        }
        i++;
    }

}

function randomizeSnowflakes() {
    // Get all elements with the class "snow__flake"
    var snowflakes = document.getElementsByClassName("snow__flake");

    // Loop through the snowflakes
    for (var i = 0; i < snowflakes.length; i++) {
      // Generate a random number between 0 and 4
      var delay = Math.random() * 4;

      // Set the "animation-delay" property for the current snowflake
      snowflakes[i].style.setProperty("animation-delay", delay + "s");
    }
}
randomizeSnowflakes();
