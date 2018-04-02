<!-- src/components/BlogListEntry.vue -->
<template>
    <div class="BlogListEntry">
        <div class="Title">
            <router-link :to="BlogUrl">
                {{title}}
            </router-link>
        </div>
        <div class="Date">{{date}}</div>
    </div>
</template>

<script lang="ts">
import Vue from "vue";

export default Vue.extend({
    props: ['title', 'date', 'id'],
    computed: {
        BlogUrl(): string {
            return '/blog/' + this.id;
        }
    }
});
</script>

<style>
</style>