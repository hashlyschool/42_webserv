#! /bin/bash
echo 'Content-Type: text/html'
echo ''
echo '<h1>Date & Calendar & Env</h1>'
echo '<h2>Calendar</h2>'
echo '<pre>'
cal
echo '</pre>'
echo '<h2>Date</h2>'
echo '<pre>'
date
echo '</pre>'
echo '<h2>Env</h2>'
echo '<pre>'
env
echo '</pre>'
echo ''
