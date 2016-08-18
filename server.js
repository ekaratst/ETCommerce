var webpack = require('webpack');
var webpackDevMiddleware = require('webpack-dev-middleware');
var webpackHotMiddleware = require('webpack-hot-middleware');
var config = require('./webpack.config');
var bodyParser = require('body-parser');
var express = require('express');
var app = new express();
var jsonFile = require('jsonfile');
var port = 8080;

var compiler = webpack(config);
app.use(bodyParser.urlencoded({ extended: false }))
app.use(express.static(__dirname + '/'));
app.use(webpackDevMiddleware(compiler, { noInfo: true, publicPath: config.output.publicPath }));
app.use(webpackHotMiddleware(compiler));

app.get("/", function(req, res) {
  res.sendFile(__dirname + '/index.html');
});

app.get('/getData', function(req, res) {
  var file = 'data.json';
  jsonFile.readFile(file, function(err ,obj) {
    res.send(obj);
  });
})
app.post('/saveData', function(req, res) {
  var file = 'data.json';
  console.log(req.body);
  console.log(req.body);
  var newData = req.body.data;
  jsonFile.writeFile(file,newData, function(err) {
    console.log(err);
  });
});
app.listen(port, function(error) {
  if (error) {
    console.error(error);
  } else {
    console.info("==> 🌎  Listening on port %s. Open up http://localhost:%s/ in your browser.", port, port);
  }
});
