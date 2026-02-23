static const char advanced_options[] = R"EOT(
<!doctype html>
<html lang="en">
<head>
    <title>esp32cam WifiCam example</title>
    <style>
        table, th, td { border: solid 1px #000000; border-collapse: collapse; }
        th, td { padding: 0.4rem; }
        a { text-decoration: none; }
        footer { margin-top: 1rem; }
    </style>
</head>
<body>
    <h1>esp32cam WifiCam example</h1>
    <table>
        <thead>
            <tr><th>BMP</th><th>JPG</th><th>MJPEG</th></tr>
        </thead>
        <tbody id="resolutions">
            <tr><td colspan="3">loading...</td></tr>
        </tbody>
    </table>
    <footer>Powered by <a href="https://esp32cam.yoursunny.dev/">esp32cam</a></footer>
    <script type="module">
        async function fetchText(uri, init) {
            const response = await fetch(uri, init);
            if (!response.ok) {
                throw new Error(await response.text());
            }
            return (await response.text()).trim().replaceAll("\r\n", "\n");
        }

        try {
            const list = (await fetchText("/resolutions.csv")).split("\n");
            document.querySelector("#resolutions").innerHTML = list.map((r) => `<tr>${
                ["bmp", "jpg", "mjpeg"].map((fmt) => `<td><a href="/${r}.${fmt}">${r}</a>`).join("")
            }`).join("");
        } catch (err) {
            document.querySelector("#resolutions td").textContent = err.toString();
        }
    </script>
</body>
</html>
)EOT";