$('#pressed').on('click', function() {
   $.ajax({
      type: "post",
      url: '/led_pwm_ajax',
      data: {range_pwm: $('#range_pwm').val()}, // + 'j'
      beforeSend: function(xhttp) {
         xhttp.overrideMimeType( "text/plain" );
      },
      success: function(data) {
         $('#response_data').html(data)
      }
   });
});