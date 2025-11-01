// File save functionality
function saveFileFromMEMFSToDisk(memoryFSname, localFSname) {
  // This can be called by C/C++ code
  var isSafari = false; // Not supported, navigator.userAgent access is being restricted
  //var isSafari = /^((?!chrome|android).)*safari/i.test(navigator.userAgent);
  var data = FS.readFile(memoryFSname);
  var blob;

  if (isSafari) blob = new Blob([data.buffer], { type: "application/octet-stream" });
  else blob = new Blob([data.buffer], { type: "application/octet-binary" });

  // NOTE: SaveAsDialog is a browser setting. For example, in Google Chrome,
  // in Settings/Advanced/Downloads section you have a setting:
  // 'Ask where to save each file before downloading' - which you can set true/false.
  // If you enable this setting it would always ask you and bring the SaveAsDialog
  saveAs(blob, localFSname);
}

// Module configuration (executed before Emscripten code)
// Note: We initialize Module as a global object that Emscripten will extend
if (typeof Module === "undefined") {
  Module = {};
}

// Configure Module properties before Emscripten code runs
Module.preRun = Module.preRun || [];
Module.postRun = Module.postRun || [];

Module.print = (function () {
  return function (text) {
    if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(" ");
    console.log(text);

    var element = document.querySelector("#output");
    if (element) {
      if (element.value === undefined) element.value = ""; // Clear browser cache on first use
      element.value += text + "\n";
      element.scrollTop = element.scrollHeight; // focus on bottom
    }
  };
})();

Module.printErr = function (text) {
  if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(" ");
  console.error(text);
};

Module.canvas = (function () {
  var canvas = document.querySelector("#canvas");

  // As a default initial behavior, pop up an alert when webgl context is lost.
  // To make your application robust, you may want to override this behavior before shipping!
  // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
  canvas.addEventListener(
    "webglcontextlost",
    function (e) {
      alert("WebGL context lost. You will need to reload the page.");
      e.preventDefault();
    },
    false
  );

  return canvas;
})();

Module.setStatus = function (text) {
  if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: "" };
  if (text === Module.setStatus.last.text) return;

  var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
  var now = Date.now();

  if (m && now - Module.setStatus.last.time < 30) return; // If this is a progress update, skip it if too soon

  Module.setStatus.last.time = now;
  Module.setStatus.last.text = text;

  var statusElement = document.querySelector("#status");
  var progressElement = document.querySelector("#progress");
  var spinnerElement = document.querySelector("#spinner");

  if (m) {
    text = m[1];
    if (progressElement) {
      progressElement.value = parseInt(m[2]) * 100;
      progressElement.max = parseInt(m[4]) * 100;
      progressElement.hidden = true;
    }
    if (spinnerElement) spinnerElement.hidden = false;
  } else {
    if (progressElement) {
      progressElement.value = null;
      progressElement.max = null;
      progressElement.hidden = true;
    }
    if (spinnerElement && !text) spinnerElement.style.display = "none";
  }

  if (statusElement) statusElement.innerHTML = text;
};

Module.totalDependencies = 0;

Module.monitorRunDependencies = function (left) {
  this.totalDependencies = Math.max(this.totalDependencies, left);
  Module.setStatus(
    left
      ? "Preparing... (" + (this.totalDependencies - left) + "/" + this.totalDependencies + ")"
      : "All downloads complete."
  );
};

Module.setStatus("Downloading...");

// Handle global errors
window.onerror = function () {
  Module.setStatus("Exception thrown, see JavaScript console");
  var spinnerElement = document.querySelector("#spinner");
  if (spinnerElement) spinnerElement.style.display = "none";
  Module.setStatus = function (text) {
    if (text) Module.printErr("[post-exception status] " + text);
  };
};
