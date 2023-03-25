timeout = 500;

const MAP = {
   "0": "OFF",
   "1": "ON"
};

function start() {
   setTimeout(() => {
      $.ajax({
         type: "get",
         url: "/stored_values",
         success: function(data) {
            for (i = 0; i < data.length; ++i) {
               if (!document.getElementById(`led${i}`)) {
                  $("body").append(`<div id="led${i}str">LED ${i}:</div>`);
                  $("body").append(`<div id="led${i}">${MAP[data[i]]}</div>`); // or $("#body")
               } else {
                  $(`#led${i}`).html(`${MAP[data[i]]}`);
               }
            }
         }
      });
      start();
   }, timeout);
}

start();

$('#verify').on('click', function() {
   parsed = parseInt($('#timeout').val())
   if (parsed && Number.isInteger(parsed)) {
      timeout = parsed;
   } else {
      alert("Incorrect input data");
   }
});