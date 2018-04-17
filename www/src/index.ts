import Vue from "vue";
import VueRouter from 'vue-router';

import Blog from "./components/Blog.vue";
import BlogList from "./components/BlogList.vue";
import BlogListEntry from "./components/BlogListEntry.vue";

import "bootstrap/dist/css/bootstrap.min.css"

Vue.use(VueRouter);

const router = new VueRouter({
    mode: 'history',
    routes: [
        { path: '/', component: BlogList },
        { path: '/blog/:id', component: Blog, props: true }
    ]
});

let v = new Vue({
    router,
    el: "#app",
    template: `
    <div class="container">
        <div class="row">
            <div class="col"></div>
            <div class="col-6 text-center">
                <router-link to="/">
                    <img src="/images/logo.png" class="img-fluid w-75" />
                </router-link>
            </div>
            <div class="col"></div>
        </div>
        <div class="row">
            <div class="col"></div>
            <div class="col-6">
                <router-view class="view"></router-view>
            </div>
            <div class="col"></div>
        </div>
    </div>
    `,
    data: { },
    components: {
        BlogList,
        BlogListEntry,
        Blog
    }
});
