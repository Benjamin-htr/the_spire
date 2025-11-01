// Audio management
// REF: https://developers.google.com/web/updates/2018/11/web-audio-autoplay

var audioBtn = document.querySelector("#btn-audio");

// An array of all contexts to resume on the page
const audioContexList = [];
(function () {
  // A proxy object to intercept AudioContexts and
  // add them to the array for tracking and resuming later
  self.AudioContext = new Proxy(self.AudioContext, {
    construct(target, args) {
      const result = new target(...args);
      audioContexList.push(result);
      if (result.state == "suspended") audioBtn.textContent = "🔈 RESUME";
      return result;
    },
  });
})();

window.toggleAudio = function toggleAudio() {
  var resumed = false;
  audioContexList.forEach((ctx) => {
    if (ctx.state == "suspended") {
      ctx.resume();
      resumed = true;
    } else if (ctx.state == "running") ctx.suspend();
  });

  if (resumed) audioBtn.textContent = "🔇 SUSPEND";
  else audioBtn.textContent = "🔈 RESUME";
};
