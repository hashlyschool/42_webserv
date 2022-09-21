from datetime import datetime

ts = datetime.now()

print("Content-type: text/html")
print("")

print("""
<html>
    <head>
        <link rel="icon" type="image/png" href="../pink.ico"/>
    </head>
    <body>
        <center>
            <br />
            <h1>Time</h1>
            <br />
            <h2 id="time_box"></h2>
        </center>
        <script>
            function setDateTime()
            {
                let date = new Date();
                let month = date.getMonth() + 1;
                if (month < 10)
                    month = "0" + month;
                let day = date.getDate();
                if (day < 10)
                    day = "0" + day;
                let s = date.getFullYear() + "-" + month + "-" + day;
                let hours = date.getHours();
                if (hours < 10)
                    hours = "0" + hours;
                let minutes = date.getMinutes();
                if (minutes < 10)
                    minutes = "0" + minutes;
                let seconds = date.getSeconds();
                if (seconds < 10)
                    seconds = "0" + seconds;
                s += " " + hours + ":" + minutes + ":" + seconds;
                time_box.innerHTML = s;
                /*var xhr = new XMLHttpRequest();
                xhr.open('GET', '/cgi-bin/api_time.py', false);
                xhr.send();
                if (xhr.status != 200) {
                  alert( xhr.status + ': ' + xhr.statusText );
                } else {
                  time_box.innerHTML = xhr.responseText;
                }*/
            }
            setDateTime();
            setInterval(setDateTime, 1000);
        </script>
    </body>
</html>
"""  # % (
     # ts.strftime("%Y-%m-%d %H:%M:%S")
     # )
)
