const express = require('express');

const app = express();
const PORT = process.env.PORT;

app.use(express.json());

const routes = require('/app/src/routes');
app.use(routes);

// As our server to listen for incoming connections
app.listen(PORT, () => console.log(`Server listening on port: ${PORT}`));