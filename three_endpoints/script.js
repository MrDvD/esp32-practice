function start() {
   setTimeout(function() {
      $.ajax({
         type: "get",
         url: '/values',
         success: function(data) {
            $('#led1').html(data[0]);
            $('#led2').html(data[1]);
         }
      });
     start();
   }, 500);
}
start();