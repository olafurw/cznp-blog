<!-- src/components/Note.vue -->
<template>
    <div v-if="HasNote">
        <div class="Note">
            <div class="NoteHeader">
                <span class="Title">{{myNote.title}}</span>
                <span class="Date"> - {{myNote.date}}</span>
            </div>
            <div v-html="myNote.text"></div>
        </div>
    </div>
</template>

<script lang="ts">
import Vue from "vue";
import Component from 'vue-class-component'
import { Emit, Prop, Watch } from 'vue-property-decorator'
import axios from "axios"
import * as showdown from "showdown";
import * as showdownHighlight from "showdown-highlight";
const locConverter = new showdown.Converter({ ghCodeBlocks: false, extensions: [showdownHighlight] });

import { INote } from '../data/INote'

export default Vue.extend({
    props: ['id'],
    data: function()
    {
        return {
            myNote: null,
        }
    },
    created: function()
    {
        axios.get('/note-data/' + this.id)
        .then((aResponse: any) =>
        {
            this.myNote = aResponse.data;

            if (this.myNote != null
                && this.myNote.text != null
                && this.myNote.text != "")
            {
                this.myNote.text = locConverter.makeHtml(this.myNote.text);
            }
        })
        .catch((aError: any) =>
        {
            console.log(aError);
        });
    },
    computed:
    {
        HasNote(): boolean
        {
            return this.myNote !== null;
        }
    }
});
</script>

<style>
</style>