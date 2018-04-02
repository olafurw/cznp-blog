<!-- src/components/Blog.vue -->
<template>
    <div v-if="HasBlog">
        <div class="Blog">
            <div>{{myBlog.title}}</div>
            <div>{{myBlog.text}}</div>
        </div>
    </div>
</template>

<script lang="ts">
import Vue from "vue";
import Component from 'vue-class-component'
import { Emit, Prop, Watch } from 'vue-property-decorator'
import axios from "axios"

import { IBlog } from '../data/IBlog'

export default Vue.extend({
    props: ['id'],
    data: function()
    {
        return {
            myBlog: null,
        }
    },
    created: function()
    {
        axios.get('/blog-data/' + this.id)
        .then((aResponse: any) =>
        {
            this.myBlog = aResponse.data;
        })
        .catch((aError: any) =>
        {
            console.log(aError);
        });
    },
    computed:
    {
        HasBlog(): boolean
        {
            return this.myBlog !== null;
        }
    }
});
</script>

<style>
</style>