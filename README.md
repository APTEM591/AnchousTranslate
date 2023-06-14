# AnchousTranslate
A simple telegram bot written on c++ for translating things from one language to another.

Uses Google Translation API to automatically translate anything you type. Limit is 1000 characters per hour due to API limits.

Required TG bot token and Google cloud API key to get this bot to work.

## Usage examples:

Note: you need to specify target language and source text only, the source language will be detected automatically.

### input: "/translate en Привіт Світ!"
```
uk: Привіт Світ!
-------------
en: Hello World!
```

### input: "/translate fr Hello World!"
```
en: Hello World!
-------------
fr: Bonjour le monde!
```

### input: "/limit"
```
You have 395 characters left for this hour
```
