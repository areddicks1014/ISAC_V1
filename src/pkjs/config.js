module.exports = [
  { 
    "type": "heading", 
    "size": "3",
    "defaultValue": "ISAC Configurations" 
  },
  {
    "type": "section",
    "items": [
      {
      "type": "toggle",
      "messageKey": "mode",
      "defaultValue": "false",
      "label": "Color Mode",
      "description": "Dynamic mode changes the color depending on status. Static mode changes the color based on the options below."
      },
      {
        "type": "heading",
        "defaultValue": "Theme Color"
      },
      { 
        "type": "text", 
        "defaultValue": "Choose a default theme color inspired by The Division." 
      },
      {
      "type": "select",
      "messageKey": "theme",
      "defaultValue": "tech",
      "label": "Division Theme",
      "options": [
        { 
          "label": "Tech [Orange]", 
          "value": "tech" 
        },
        { 
          "label": "Security [Blue]",
          "value": "security" 
        },
        { 
          "label": "Medical [Green]",
          "value": "medical" 
        },
        { 
          "label": "Dark Zone [Purple]",
          "value": "dark_zone" 
        },
        {
          "label": "Rogue [Red]",
          "value": "rogue"
        },
        {
          "label": "Custom [Choose Below]",
          "value": "custom"
        }
        ]
      },
      {
        "type": "color",
        "messageKey": "custom",
        "defaultValue": "ff5500",
        "sunlight": false,
        "label": "Custom Color",
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
];