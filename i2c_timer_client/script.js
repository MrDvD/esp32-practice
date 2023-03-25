timeout = 500;

function start() {
   setTimeout(() => {
      $.ajax({
         type: "post",
         url: "/timeout",
         headers: {"Content-Type": "text/plain"},
         data: {"timeout": timeout},
         success: function(data) {
            $('#analog').html(data);
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