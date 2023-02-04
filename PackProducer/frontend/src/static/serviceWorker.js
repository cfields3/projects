const STATIC_CACHE_NAME = 'packproducer-static-v0';

self.addEventListener('install', event => {
  event.waitUntil(
    caches.open(STATIC_CACHE_NAME).then(cache => {
      return cache.addAll([
        '/',
        '/offline',
        //CSS
        '/css/base.css',
        '/css/error.css',
        '/css/login.css',
        '/css/explore.css',
        '/css/post.css',
        '/css/message.css',
        //Images
        '/img/error.png',
        '/img/network-error.png',
        '/img/icon_512x512.png',
        //Scripts
        '/js/APIClient.js',
        '/js/base.js',
        '/js/HTTPClient.js',
        '/js/login.js',
        '/js/explore.js',
        '/js/post.js',
        '/js/message.js',
      ]);
    })
  );
});

self.addEventListener('activate', event => {
  event.waitUntil(
    caches.keys().then(cacheNames => {
      return Promise.all(
        cacheNames.filter(cacheName => {
          return cacheName.startsWith('packproducer-') && cacheName != STATIC_CACHE_NAME;
        }).map(cacheName => {
          return caches.delete(cacheName);
        })
      );
    })
  );
});

self.addEventListener('fetch', event => {
  var requestUrl = new URL(event.request.url);
  //Treat API calls (to our API) differently
  if(requestUrl.origin === location.origin && requestUrl.pathname.startsWith('/api')) {
    //If we are here, we are intercepting a call to our API
    if(event.request.method === "GET") {
      //Only intercept (and cache) GET API requests
      event.respondWith(
        networkFirst(event.request)
      );
    }
  }
  else {
    //If we are here, this was not a call to our API
    event.respondWith(
      networkFirst(event.request)
    );
  }

});

async function networkFirst(request) {
  let cache = await caches.open(STATIC_CACHE_NAME);

  try {
      let response = await fetch(request)
      cache.put(request, response.clone())
      return response;
  } catch {
      let cachedResponse = await cache.match(request);
      if (cachedResponse) {
          return cachedResponse;
      } else {
          return cache.match("/offline");
      }
  }
}

self.addEventListener('message', event => {
  if(event.data.action === 'skipWaiting') {
    self.skipWaiting();
  }
});
