<!-- src/components/About.vue -->
<template>
    <div v-if="HasAbout">
        <div class="About">
            <div v-html="myAbout"></div>
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

export default Vue.extend({
    data: function()
    {
        return {
            myAbout: null,
        }
    },
    created: function()
    {
        axios.get('/about-data/')
        .then((aResponse: any) =>
        {
            this.myAbout = aResponse.data;

            if (this.myAbout != null
                && this.myAbout != "")
            {
                this.myAbout = locConverter.makeHtml(this.myAbout);
            }
        })
        .catch((aError: any) =>
        {
            console.log(aError);
        });
    },
    computed:
    {
        HasAbout(): boolean
        {
            return this.myAbout !== null;
        }
    }
});
</script>

<style>
</style>