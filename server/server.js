const express = require('express');
const path = require('path');
const fs = require('fs');
const app = express();
const port = 3000;

const outputFilePath = path.join(__dirname, '..', 'build', 'output.json');
let jsonData = [];

try {
    const data = fs.readFileSync(outputFilePath, 'utf8');
    jsonData = JSON.parse(data);
} catch (err) {
    console.error('Error reading or parsing output.json file:', err);
}

const extractAttribute = (attribute) => {
    return jsonData.map(entry => entry[attribute]);
};

app.get('/output', (req, res) => {
    res.json(jsonData);
});

app.get('/output/time', (req, res) => {
    res.json(extractAttribute('Time'));
});

app.get('/output/latitude', (req, res) => {
    res.json(extractAttribute('Latitude'));
});

app.get('/output/longitude', (req, res) => {
    res.json(extractAttribute('Longitude'));
});

app.get('/output/fixquality', (req, res) => {
    res.json(extractAttribute('Fix Quality'));
});

app.get('/output/numberofsatellites', (req, res) => {
    res.json(extractAttribute('Number of Satellites'));
});

app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`);
});
