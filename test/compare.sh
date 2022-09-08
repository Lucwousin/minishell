#!/bin/bash

while read line; do
  if [ "$line" == "stop" ]; then
    break
  fi

  echo "minishell:"
  eval "./minishell -c '$line'"
  echo "minishell returned $?"

  echo

  echo "bash:"
  eval "bash -c '$line'"
  echo "bash returned $?"
done
