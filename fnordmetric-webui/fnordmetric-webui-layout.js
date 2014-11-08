/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Laura Schlimmer
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
if (FnordMetric === undefined) {
  FnordMetric = {};
}

if (FnordMetric.views === undefined) {
  FnordMetric.views = {};
}

FnordMetric.WebUI = function() {
  var routes = {};
  var current_view = null;

  var viewport = document.createElement("div");
  viewport.className = "viewport";
  var headbar = document.createElement("div");
  headbar.className = "headbar";

  function addMenuItem(name, url) {
    var menuitem = document.createElement("a");
    menuitem.href = "#" + url;
    menuitem.innerHTML = "<h1>" + name + "</h1>";
    headbar.appendChild(menuitem);
    menuitem.addEventListener('click', function(e) {
      e.preventDefault();
      openUrl(this.getAttribute("href").substr(1), true);
      return false;
    });
  }

  function init() {
    document.body.appendChild(headbar);
    document.body.appendChild(viewport);

    // initialize the popstate handler
    window.onpopstate = function(e) {
      e.preventDefault();

      if (e.state != null && typeof e.state.url != "undefined") {
        openUrl(e.state.url);
      }
    }
  };

  function addView(route, view) {
    routes[route] = view;
  }

  function renderView(view, url) {
    if (current_view != null) {
      current_view.destroy(viewport);
    }

    FnordMetric.util.displayLoader(viewport);

    current_view = view;
    view.render(viewport, url);
  };

  function openUrl(raw_url, push_state) {
    var url = FnordMetric.util.parseQueryString(raw_url);

    view = routes[url["path"]];
    if (view === undefined) {
      alert("no route found for: " + url["path"]);
      return;
    }

    if (typeof push_state == "undefined" || push_state === true) {
      window.history.pushState({url: raw_url}, "", "#" + raw_url);
    }

    renderView(view(), url);
  }

  init();

  return {
    "openUrl" : openUrl,
    "addView": addView,
    "addMenuItem": addMenuItem
  }
};