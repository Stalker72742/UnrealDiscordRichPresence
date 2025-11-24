# Unreal Discord Presence

A plugin for Unreal Engine that displays Discord Rich Presence while you're working in the editor. Let your friends know you're deep in development instead of ignoring their messages.

**Supported Engine Versions:** Unreal Engine 5.6

## Features

- Automatically shows Discord presence when the editor launches
- Displays your current project name
- Shows how long you've been suffering in the editor
- Fully customizable through Project Settings

## Installation

### Prerequisites
Download the Discord Social SDK from the Discord Developer Portal:
1. Go to https://discord.com/developers/applications
2. Select your application (or create one)
3. Navigate to "Discord Social SDK" in the sidebar
4. Download the latest Discord social SDK plugin release
5. Copy the plugin to your project's `Plugins` folder

### Plugin Setup
1. Copy the plugin to your project's `Plugins` folder
2. Enable the plugin if it doesn't enable automatically


## Configuration

**Edit → Project Settings → Plugins → Discord Presence**

| Parameter | Description |
|-----------|-------------|
| **Enable Discord Presence** | Toggle the plugin on/off |
| **Application ID** | Your Discord application ID (get one from [Discord Developer Portal](https://discord.com/developers/applications)) |
| **State Text** | Custom status text (defaults to project name) |
| **Details Text** | Additional description, e.g. "Working on gameplay" |

## Requirements

- Unreal Engine 5.6+
- Discord Desktop client (browser version doesn't support Rich Presence)

## License

MIT
