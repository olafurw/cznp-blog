import Vue from "vue";
import VueRouter from 'vue-router';

import Blog from "./components/Blog.vue";
import BlogList from "./components/BlogList.vue";
import BlogListEntry from "./components/BlogListEntry.vue";

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
    <div>
        <router-view class="view"></router-view>
    </div>
    `,
    data: { },
    components: {
        BlogList,
        BlogListEntry,
        Blog
    }
});
