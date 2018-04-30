import Vue from "vue";
import VueRouter from 'vue-router';

import Note from "./components/Note.vue";
import NoteList from "./components/NoteList.vue";
import NoteListEntry from "./components/NoteListEntry.vue";
import About from "./components/About.vue";

import "bootstrap/dist/css/bootstrap.min.css"
import "highlight.js/styles/zenburn.css"
import "./css/cznp.css"

Vue.use(VueRouter);

const router = new VueRouter({
    mode: 'history',
    routes: [
        { path: '/', component: NoteList },
        { path: '/note/:id', component: Note, props: true },
        { path: '/about', component: About, props: true }
    ]
});

let v = new Vue({
    router,
    el: "#app",
    template: `
    <div class="container">
        <div class="row">
            <div class="col"></div>
            <div class="col-8">
                <router-link to="/">
                    <h1 class="logo">olafurw</h1>
                </router-link>
            </div>
            <div class="col"></div>
        </div>
        <div class="row">
            <div class="col"></div>
            <div class="col-8 text-center">
                <nav class="navbar navbar-expand-lg navbar-dark bg-primary">
                    <div class="collapse navbar-collapse">
                        <ul class="navbar-nav mr-auto">
                            <li class="nav-item">
                                <router-link to="/" class="nav-link nav-color-override">Notes</router-link>
                            </li>
                            <li class="nav-item">
                                <router-link to="/about" class="nav-link nav-color-override">About</router-link>
                            </li>
                        </ul>
                    </div>
                </nav>
            </div>
            <div class="col"></div>
        </div>
        <div class="row">
            <div class="col"></div>
            <div class="col-8">
                <router-view class="view"></router-view>
            </div>
            <div class="col"></div>
        </div>
    </div>
    `,
    data: { },
    components: {
        NoteList,
        NoteListEntry,
        Note,
        About
    }
});
