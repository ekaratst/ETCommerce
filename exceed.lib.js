(function() {
  'use strict';
  window.EXcreateMessage = function(message, username) {
    return {
      id: new Date().getTime(),
      user: username,
      message: message
    };
  };
  window.EXformat = function(arr) {
    var str = JSON.stringify(arr);
    return str.substring(0,str.length);
  };
}());
