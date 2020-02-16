$(document).ready(function(){

  // Country
  $('#name').change(function(){

    var str = $(this).val();
    var array = str.split("_");
    i=0;

    array.forEach(myFunction);

    function myFunction(item) {
      $("#"+i).val(item);
      i+=1;
    }
  });

  $('#save').click(function(){
    $("#0").prop('disabled', false);

  });

});
