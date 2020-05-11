/*var i = 4;
var s = i.toString;
i+="15";
var s1 = i.toString;
s+=s1;
Response.write(s);
Response.write(i);
Response.write("here \"it\" is");*/
var i;
var k = 15;
var header = "Тестовая страница на MJS";
var s = 0;
for(i = 1; i <= k; i++){
s = s + i;
}
write("<html>");
write("<meta charset=\"utf-8\">");
write("<body>");
var out = "<h1>" + header + "</h1>";
write(out);
out = "<p>Параметры запроса: " + Environment.QUERY_STRING + "</p>";
write(out);
out = "<p>Сумма чисел от 1 до " + k + ": " + s + "</p>";
write(out);
if(s > 320){
write("<p>Больше 320</p>");
}else{
write("<p>Меньше 320</p>");
}
write("</body>");
write("</html>");
