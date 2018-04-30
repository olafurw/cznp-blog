"use strict";

var decodeHtml = require("html-encoder-decoder").decode,
    showdown = require("showdown"),
    hljs = require("highlight.js");

module.exports = function showdownHighlight() {
    return [{
        type: "output",
        filter: function filter(text, converter, options)
        {
            var left = "<code>[a-z]+";
            var right = "</code>";
            var flags = "g";
            
            var replacement = function replacement(wholeMatch, match, left, right)
            {
                match = decodeHtml(match);

                const lang = left.slice(6);
                if (lang)
                {
                    return "<pre class='hljs'>" + hljs.highlight(lang, match).value + "</pre>" + right;
                }
                else
                {
                    return "<pre class='hljs'>" + hljs.highlightAuto(match).value + "</pre>" + right;
                }
            };

            return showdown.helper.replaceRecursiveRegExp(text, replacement, left, right, flags);
        }
    }];
};