#!/usr/bin/env bash
set -e

echo "🚀 Setting up Corepy development environment..."

# Color codes
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# 1. Install uv if needed
if ! command -v uv &> /dev/null; then
    echo -e "${YELLOW}⚠️  uv not found. Installing uv...${NC}"
    curl -LsSf https://astral.sh/uv/install.sh | sh
    export PATH="$HOME/.cargo/bin:$PATH"
fi

# 2. Install Rust if needed
if ! command -v cargo &> /dev/null; then
    echo -e "${YELLOW}⚠️  Rust not found. Installing Rust...${NC}"
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    source "$HOME/.cargo/env"
fi

echo -e "${BLUE}📦 Syncing environment (Runtime + Dev + Docs)...${NC}"
# Installs everything defined in pyproject.toml [dependency-groups] and [optional-dependencies]
# This replaces manual pip installs and handles the editable install of 'corepy'
uv sync --all-extras --group dev

echo -e "${BLUE}🦀 Building Rust runtime (via maturin)...${NC}"
# We explicitly build the runtime to ensure bindings are up to date
cd rust/corepy-runtime
uv run maturin develop --release
cd ../..

echo -e "${GREEN}✅ Development environment setup complete!${NC}"
echo ""
echo "Next steps:"
echo "  1. Activate: source .venv/bin/activate"
echo "  2. Test:     uv run pytest"
