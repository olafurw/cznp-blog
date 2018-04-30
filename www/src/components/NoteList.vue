<!-- src/components/NoteList.vue -->
<template>
    <div>
        <div v-if="HasNoteList()">
            <div v-for="note in myNoteList">
                <NoteListEntry :title="note.title" :date="note.date" :id="note.id" />
            </div>
        </div>
    </div>
</template>

<script lang="ts">
import Vue from "vue";
import Component from 'vue-class-component'
import { Emit, Prop, Watch } from 'vue-property-decorator'
import axios from "axios"

import NoteListEntry from "./NoteListEntry.vue";

import { INoteList } from '../data/INoteList'

@Component({
    components: {
        NoteListEntry
    }
})
export default class NoteList extends Vue
{
    myNoteList: INoteList[] | null = null;

    HasNoteList(): boolean
    {
        return this.myNoteList !== null;
    }

    created()
    {
        axios.get('/note-list/')
        .then((aResponse: any) =>
        {
            if (!aResponse.data)
            {
                return;
            }

            this.myNoteList = aResponse.data;
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