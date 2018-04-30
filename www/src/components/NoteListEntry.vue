<!-- src/components/NoteListEntry.vue -->
<template>
    <div class="NoteListEntry">
        <div class="Title">
            <router-link :to="NoteUrl">
                {{title}}
            </router-link>
            <span class="Date"> - {{date}}</span>
        </div>
    </div>
</template>

<script lang="ts">
import Vue from "vue";

export default Vue.extend({
    props: ['title', 'date', 'id'],
    computed: {
        NoteUrl(): string {
            return '/note/' + this.id;
        }
    }
});
</script>

<style>
</style>