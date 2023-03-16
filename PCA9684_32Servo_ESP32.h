const char pageHeader_p1[] PROGMEM = R"header_p1(
<!DOCTYPE html>
<html><head>
  <title>Robojax PCA9685 ESP32 Servo Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <script>
    const d = document,
          qry  = (...x) => d.querySelector(...x);
          qrys = (...x) => d.querySelectorAll(...x);

    addEventListener("load", ()=>{
      qry("#allServo").addEventListener("click", e => {
        if(e.innerText == "Start All Servo"){
          fetch("servo?do=start");
          e.innerHTML = "Stop All Servo";
        } else {
          fetch("servo?do=stop");
          e.innerHTML = "Start All Servo";
        }
      });
      qrys("#ctrl td.servo").forEach(s => {
        const [disp, decBtn, input, incBtn] = [...s.children];
        decBtn.addEventListener("click", ()=>{
          input.value--;
          input.dispatchEvent(new Event("change"));
        });
        incBtn.addEventListener("click", ()=>{
          input.value++;
          input.dispatchEvent(new Event("change"));
        });
        input.addEventListener("change", e => {
          disp.innerHTML = "" + input.value;
          fetch("servo?" + new URLSearchParams({
            servo: s.dataset.servo,
            deg: input.value
          }));
        })
      });
    });
  </script>
  <style>
    * { box-sizing: border-box }
    td.servo span {
      display: inline-block;
      min-width: 4ch;
      text-align: center;
    }
  </style>
</head><body>
  <h1>Robojax PCA9685 ESP32 Servo</h1>
)header_p1";


////////////////for motor 1 part 1///////////////
const char servoControl_p1[] PROGMEM = R"body_p1(




)body_p1";
