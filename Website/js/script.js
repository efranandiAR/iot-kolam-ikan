// $(document).ready(function () {
//   $.getJSON(
// 		"https://api.thingspeak.com/channels/1505876/status/last.json",
// 		function (fields) {
			
// 		}
// 	);
// });

// //Menghitung jumlah data
// $(document).ready(function () {
//     setInterval(function () {
//     $.getJSON('https://api.thingspeak.com/channels/1505876/feeds/last.json?', function(data) {
//       $(".value.ph").html(data.field1);
//     });
//     }, 10000);
//     }); 

// $(document).ready(function () {
//     setInterval(function () {
//     $.getJSON('https://api.thingspeak.com/channels/1505876/feeds/last.json?', function(data) {
//       $(".value.suhu").html(data.field2);
//     });
//     }, 10000);
//     });     

// $(document).ready(function () {
//     setInterval(function () {
//     $.getJSON('https://api.thingspeak.com/channels/1505876/feeds/last.json?', function(data) {
//       $(".value.konsentrasiNutrisi").html(data.field3);
//     });
//     }, 10000);
//     });     