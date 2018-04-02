<!-- src/components/BlogList.vue -->
<template>
    <div>
        <div v-if="HasBlogList()">
            <div v-for="blog in myBlogList">
                <BlogListEntry :title="blog.title" :date="blog.date" :id="blog.id" />
            </div>
        </div>
    </div>
</template>

<script lang="ts">
import Vue from "vue";
import Component from 'vue-class-component'
import { Emit, Prop, Watch } from 'vue-property-decorator'
import axios from "axios"

import BlogListEntry from "./BlogListEntry.vue";

import { IBlogList } from '../data/IBlogList'

@Component({
    components: {
        BlogListEntry
    }
})
export default class BlogList extends Vue
{
    myBlogList: IBlogList[] | null = null;

    HasBlogList(): boolean
    {
        return this.myBlogList !== null;
    }

    created()
    {
        axios.get('/blog-list/')
        .then((aResponse: any) =>
        {
            if (!aResponse.data)
            {
                return;
            }

            this.myBlogList = aResponse.data;
        })
        .catch((aError: any) =>
        {
            console.log(aError);
        });
    }
}
</script>

<style>
</style>